/**
 *  DefaultConfigs
 *
 *  Default configurations for RocksServer
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */

#pragma once

namespace RocksServer {


    struct DefaultConfigs
    {
        /**
         *  Configs HTTP 
         */

        // HTTP server host
        const char* server_host = "127.0.0.1";

        // HTTP server port
        unsigned short server_port = 5577;

        // HTTP server content_type
        // Set the value to use for the Content-Type header
        const char* content_type = "text/plain; charset=UTF-8";

        // Limitations for body size
        // limit in bytes. 0 - unlimited
        size_t max_body_size = 0;

        // Limitations for headers size
        // limit in bytes. 0 - unlimited
        size_t max_headers_size = 0;

        // Error level
        EvLogger::Level log_level = EvLogger::Level::none;


        // Error log file name
        std::string error_log = "/var/log/rocksserver/error.log";

        // Set nofile soft limit
        // This option sets the soft limit on the number of simultaneously opened files
        // (including sockets, pipe, log files, etc).
        // You can see your current soft limit by command `ulimit -Sn` and your hard limit by command `ulimit -Hn`
        // The value of the soft of limit must not exceed the value of hard limit.
        // If for some reason the server will take a lot of connections, the number of which exceeds nofile_limit, then part of the connections will not be processed.
        // Increase nofile_limit can solve the problem.
        //
        // 0 - default system value (usually 1024)
        unsigned short nofile_limit = 0;


        /**
         *  Configs RocksDB 
         */

        // RocksDB data base path
        std::string db_path = "/var/rocksserver/db";

        /**
         *  Configs backups RocksDB 
         */

        // RocksDB backup path
        // The directory in which a backup will be saved
        std::string backup_path = "/var/rocksserver/backup";

        //Is this DB backupable?
        bool isbackupable = true;

        // deletes old backups, keeping latest num_backups_to_keep alive
        // If num_backups_to_keep == 0, no specified — will keep all backups
        uint32_t num_backups_to_keep = 0;

        /**
         *  Etc 
         */

        // Path to RocksServer extentions directory
        std::string extdir = "/usr/lib/rocksserver/plugins";

    };

}
