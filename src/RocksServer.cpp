/**
 *  RocksServer.cpp
 *
 *  Simple RocksDB server
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#include "RocksServer.h"

using namespace RocksServer;

int main(int argc, char **argv)
{
    
    if (argc < 2) {
        std::cerr << "Run as:" << std::endl << argv[0] << " <config.ini file name>" << std::endl;
        return 1;
    }
    std::cerr << "RocksServer version is " << ROCKSSERVER_VERSION << std::endl;

    /**
     *  
     *  Prevent the server from crashing on signal SIGPIPE
     *  
     */    
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        return 2;
    }

    /**
     *  
     *  Init config
     *  
     */
    IniConfigs cfg(argv[1]);
    if(!cfg) {
        std::cerr<<"Failed to open file "<< argv[1] << std::endl;
        return 3;
    }
    DefaultConfigs dfCfg;

    /**
     *  
     *  Set nofile soft limit
     *   
     *  This option sets the soft limit on the number of simultaneously opened files
     *  (including sockets, pipe, log files, etc).
     *  You can see your current soft limit by command `ulimit -Sn` and your hard limit by command `ulimit -Hn`
     *  The value of the soft limit must not exceed the value of the hard limit.
     *  If for some reason the server will take a lot of connections,
     *  the number of which exceeds nofile_limit, then part of the connections will not be processed.
     *  Increase nofile_limit can solve the problem.
     *  If nofile_limit is 0, it is left as the default.
     *  
     */
    auto nofile_limit = cfg.get("nofile_limit", dfCfg.nofile_limit);
    if(auto flim_chng_result = change_files_limit(nofile_limit); !flim_chng_result) {
        std::cerr << "You tried to set the nofile soft limit to " << nofile_limit << "." << std::endl;
        std::cerr << "Current nofile soft limit value is " << flim_chng_result.soft_limit << "." << std::endl;
        std::cerr << "Current nofile hard limit value is " << flim_chng_result.hard_limit << "." << std::endl;
    }
    
    /**
     *  
     *  Changing process owner (if required)
     *  
     *  If RocksServer is launched by an unprivileged user, this adjustment has no effect.
     *  If RocksServer is launched by root you can change the process owner.
     *  Specify a uid if you would like RocksServer to change the process owner, or keep its value as 0 to stay as root.
     *  
     */
    {
        unsigned int owner_uid = cfg.get("owner_uid", 0u);
        unsigned int owner_gid = cfg.get("owner_gid", owner_uid);
        if(!change_owner(owner_uid, owner_gid)) {
            std::cerr << "Failed to change the owner to " << owner_uid << ':' << owner_gid << "." << std::endl;
            return 5;
        }
    }

    /**
     *  
     *  Init extensions
     *  
     */
    Extend ext;

    /**
     *  
     *  Init RocksDB
     *  
     */
    RocksDBWrapper rdb(cfg, dfCfg);
    // Check RocksDB started
    if (!rdb.status()) {
        std::cerr << "RocksDB start error:" << std::endl << rdb.getStatus() << std::endl;
        return 6;
    }
    std::cerr << "RocksDB version is " << ROCKSDB_MAJOR << "." << ROCKSDB_MINOR << "." << ROCKSDB_PATCH << std::endl;


    /**
     *  
     *  Init logging http server
     *  
     */
    EvLogger logger(cfg.get("log_level", dfCfg.log_level), 
                    cfg.get("error_log", dfCfg.error_log));

    // Check if logger started
    if (!logger) {
        std::cerr<<"Failed to open log file" << std::endl;
        return 7;
    }

    /**
     *  
     *  Init libevent
     *  
     */
    if (!event_init()) {
        std::cerr << "Failed to initialize libevent." << std::endl;
        return 8;
    }

    /**
     *  
     *  Init event http server
     *  
     */
    EvServer server(cfg.get("server_host", dfCfg.server_host), 
                    cfg.get("server_port", dfCfg.server_port));

    // Check if server started
    if (!server) {
        std::cerr << "Failed to init http server." << std::endl;
        return 9;
    }

    /**
     *  
     *  Set http server options
     *  
     */
    EvServerOptions serverOptions;
    // Sets the value to use for the Content-Type header when none is provided. 
    serverOptions.content_type     = cfg.get("content_type", dfCfg.content_type);
    // Sets which HTTP methods are supported in requests accepted by this server.
    serverOptions.allowed_methods  = EVHTTP_REQ_POST | EVHTTP_REQ_GET;
    // Limitations for body size (in bytes; 0 - unlimited)    
    serverOptions.max_body_size    = cfg.get("max_body_size", dfCfg.max_body_size);
    // Limitations for headers size (in bytes; 0 - unlimited)    
    serverOptions.max_headers_size = cfg.get("max_headers_size", dfCfg.max_headers_size);
    server.setOptions(serverOptions);
    
    /**
     *  
     *  Bind request listeners
     *  
     */
    server
        .bind< RequestGet           >("/get",            rdb)
        .bind< RequestMget          >("/mget",           rdb)
        .bind< RequestSet           >("/set",            rdb)
        .bind< RequestMset          >("/mset",           rdb)
        .bind< RequestKeyExist      >("/exist",          rdb)
        .bind< RequestDel           >("/del",            rdb)
        .bind< RequestMdel          >("/mdel",           rdb)
        .bind< RequestMdelset       >("/mdelset",        rdb)
        .bind< RequestIncr          >("/incr",           rdb)
        .bind< RequestTailing       >("/tail",           rdb)
        .bind< RequestStats         >("/stats",          rdb)
        .bind< RequestPrefIt        >("/prefit",         rdb)
        .bind< RequestSeekPrev      >("/seekprev",       rdb)
        .bind< RequestSeekNext      >("/seeknext",       rdb)
        .bind< RequestSeekPrevRange >("/seekprev-range", rdb)
        .bind< RequestSeekNextRange >("/seeknext-range", rdb);

    //
    // If the database is backupable
    //
    rocksdb::BackupableDBOptions bkOptions( cfg.get("backup_path", dfCfg.backup_path) );

    if(cfg.get("isbackupable", dfCfg.isbackupable)) {
        // When share_table_files is true, the backup will assume that table files with the
        // same name have the same contents. This enables incremental backups and
        // avoids unnecessary data copies.
        // If share_table_files is false, each backup will be on its own and will
        // not share any data with other backups.
        // default: true
        bkOptions.share_table_files = cfg.get("share_table_files",  bkOptions.share_table_files);
        // When sync is true, we can guarantee that you'll get a consistent backup even
        // in the event of a machine crash/reboot. Backup process is slower with sync enabled.
        // If sync == false, we don't guarantee anything on machine reboot. However,
        // chances are some of the backups are consistent.
        // Default: true
        bkOptions.sync              = cfg.get("backup_sync",        bkOptions.sync);
        // If true, it will delete any existing backups.
        // Default: false
        bkOptions.destroy_old_data  = cfg.get("backup_destroy_old", bkOptions.destroy_old_data);
        // If false, we will not back up log files. This option can be useful for backing
        // up in-memory databases where log file are persisted, but table files are in memory.
        // (See https://github.com/facebook/rocksdb/wiki/How-to-persist-in-memory-RocksDB-database%3F)
        // Default: true
        bkOptions.backup_log_files  = cfg.get("backup_log_files",   bkOptions.backup_log_files);
        // Determines how many background threads will copy files for CreateNewBackup()
        // and RestoreDBFromBackup()
        // Default: 1
        bkOptions.max_background_operations
                                    = cfg.get("backup_threads",     bkOptions.max_background_operations);

        server.bind("/backup", new RequestBackup(
                rdb, 
                bkOptions,
                cfg.get("backups_to_keep", dfCfg.num_backups_to_keep), 
                cfg.get("flush_before_backup", dfCfg.flush_before_backup)
            )
        );
        server.bind("/backup/info", new RequestBackupInfo(bkOptions) );
        server.bind("/backup/del",  new RequestBackupDel (bkOptions) );
        server.bind("/backup/mdel", new RequestBackupMdel(bkOptions) );
    }
    
    // Load extentions
    ext.load(server, rdb, cfg, dfCfg);

    /**
     *  
     *  Start the event loop
     *  
     */
    if (!server.dispatch()) {
        std::cerr << "Failed to run the message loop." << std::endl;
        return 10;
    }

    return 0;
}