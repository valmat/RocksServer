<?php
/**
 *  API for RocksServer
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */
namespace RocksServer;

class Exception extends \Exception {}

class Client{
    /**
      *   host and port
      */
    private $_host;
    private $_port;
    
    function __construct($host='localhost', $port = 5577) {
        $this->_host = $host;
        $this->_port = $port;
    }
    
    /**
      *  get value by key
      */
    public function get($key) {
        return $this->httpGet('/get', $key)->getValue();
    }
    
    /**
      *  Check if key exist
      */
    public function keyExist($key, &$val = NULL) {
        $resp = $this->httpGet('/exist', $key);
        $rez = $resp->isOk();
        $val = $rez ? $resp->getValue() : NULL;
        return $rez;
    }
    
    /**
      *  multi get values by keys
      */
    public function mget($keys) {
        return $this->httpGet('/mget', implode('&', $keys))->getMultiValue();
    }
    
    /**
      *  set value for key
      */
    public function set($key, $val) {
        return $this->httpPost('/set', "$key\n".strlen($val)."\n$val")->isOk();
    }
    
    /**
      *  multi set values for keys
      */
    public function mset($data) {
        return $this->httpPost('/mset', $this->data2str($data) )->isOk();
    }
    
    /**
      *  remove key from db
      */
    public function del($key) {
        return $this->httpPost('/del', $key)->isOk();
    }
    
    /**
      *  Multi remove keys from db
      */
    public function mdel($keys) {
        return $this->httpPost('/mdel', implode("\n", $keys) )->isOk();
    }
    
    /**
      *  incriment value by key
      */
    public function incr($key, $value = NULL) {
        return  (
                    $value ?
                    $this->httpPost('/incr', "$key&$value" )->isOk() :
                    $this->httpPost('/incr', $key )->isOk()
                );
    }
    
    /**
      *  backup database
      */
    public function backup() {
        return $this->httpPost('/backup');
        //return $this->httpPost('/backup')->isOk();
    }
    
    /**
      *  POST request
      */
    private function httpPost($path, $data = NULL) {
        $buf  = "POST $path HTTP/1.1\r\n";
        $buf .= "Host:{$this->_host}\r\n";
        
        if(NULL !== $data) {
            $buf .= "Content-Type:application/x-www-form-urlencoded; charset=UTF-8\r\n";
            $buf .= "Content-Length: " . strlen($data) . "\r\n";
            $buf .= "Connection: Close\r\n\r\n";
            $buf .= $data;
        } else {
            $buf .= "Content-Type: charset=UTF-8\r\n";
            $buf .= "Connection: Close\r\n\r\n";
        }
        return $this->request($buf);
    }
    
    /**
      *  GET request
      */
    private function httpGet($path, $data) {
        $buf  = "GET $path?$data HTTP/1.1\r\n";
        $buf .= "Host:{$this->_host}\r\n";
        $buf .= "Content-Type:charset=UTF-8\r\n";
        $buf .= "Connection: Close\r\n\r\n";
        return $this->request($buf);
    }
    
    /**
      *  @param string request
      */
    private function request(&$req) {
        if( !($this->sock = fsockopen($this->_host, $this->_port, $errno, $errstr)) ){
            throw new Exception("Unable to create socket: $errstr ($errno)");
        }
        fwrite($this->sock, $req);
        
        // skip headers
        $s = '';
        while (!feof($this->sock) && "\r\n" !== $s) {
            $s =  fgets($this->sock);
        }
        
        return new Response($this->sock);
    }
    
    /**
      *  Encodes data to send in a POST request
      */
    private function data2str(array $data) {
        $ret = '';
        foreach($data as $key => $val) {
            $ret .= "$key\n".strlen($val)."\n$val\n";
        }
        return $ret;
    }
    
}
