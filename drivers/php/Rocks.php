<?php
/**
  *  API for RocksServer 
  */
namespace Rocks;

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
      *   
      */
    private function request($path, $method, $data = '') {
        
        if ($path{0} != '/') {
            $url = "http://{$this->_host}:{$this->_port}/" . $path;
        } else {
            $url = "http://{$this->_host}:{$this->_port}" . $path;
        }
        
        return ('POST' == $method) ? $this->httpPost($url, $data) : $this->httpGet($url, $data);
    }
    
    /**
      *  POST request
      */
    private function httpPost($path, $data = NULL) {
        if(NULL != $data) {
            $opts = array(
                'http'=>array(
                    'method'=> "POST",
                    'header'=> "Host:{$this->_host}\r\n" .
                               "Content-Type:application/x-www-form-urlencoded; charset=UTF-8\r\n" .
                               "Content-Length: " . strlen($data) . "\r\n",
                    'content' => $data
                )
            );
        } else {
            $opts = array(
                'http'=>array(
                    'method'=> "POST",
                    'header'=> "Host:{$this->_host}\r\n" .
                               "Content-Type: charset=UTF-8\r\n"
                )
            );
        }
        return file_get_contents("http://{$this->_host}:{$this->_port}$path", false, stream_context_create($opts));
    }
    
    /**
      *  GET request
      */
    private function httpGet($path, $data) {
        $opts = array(
            'http'=>array(
                'method'=>"GET",
                'header'=>  "Host:{$this->_host}\r\n" .
                            "Content-Type:charset=UTF-8\r\n"
            )
        );
        
        return file_get_contents("http://{$this->_host}:{$this->_port}$path?$data", false, stream_context_create($opts));
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
    /**
      *  Encodes data to send in a GET request
      */
    private function keys2str(array $keys) {
        return implode('&', $keys);
    }
    
    /**
      *  get value by key
      */
    public function get($key) {
        return $this->parseGet($this->httpGet('/get', $key));
    }
    
    private function parseGet(&$data) {
        list($rezlen, $rez) = explode("\n", $data, 2);
        $rezlen = (int) $rezlen;
        if($rezlen < 0) {
            return NULL;
        }
        if(strlen($rez) > $rezlen) {
            $rez = substr($rez, 0, $rezlen);
        }
        return $rez;
    }
    
    /**
      *  multi get values by keys
      */
    public function mget($keys) {
        $raw = $this->httpGet('/mget', implode('&', $keys));
        $rawlen = strlen($raw);
        if(!$rawlen) {
            return [];
        }
        
        $lpos = 0;
        $rez = [];
        while($lpos < $rawlen) {
            
            // retrive key
            $rpos = strpos($raw, "\n", $lpos );
            $key = substr($raw, $lpos, $rpos - $lpos);
            
            // retrive value lenght
            $lpos = $rpos+1;
            $rpos = strpos($raw, "\n", $lpos );
            $vallen = (int)substr($raw, $lpos, $rpos - $lpos);
            $lpos = $rpos+1;
            
            // retrive value
            if(($vallen > 0)) {
                $rez[$key] = substr($raw, $lpos, $vallen);
                $lpos += $vallen+1;
            } else {
                $rez[$key] = NULL;
            }
        }
        
        return $rez;
    }
    
    /**
      *  set value for key
      */
    public function set($key, $val) {
        return 'OK' == $this->httpPost('/set', "$key\n".strlen($val)."\n$val");
    }
    
    /**
      *  multi set values for keys
      */
    public function mset($data) {
        return 'OK' == $this->httpPost('/mset', $this->data2str($data) );
    }
    
    /**
      *  Check if key exist
      */
    public function keyExist($key, &$val = NULL) {
        list($rez, $valRaw) = explode("\n", $this->httpGet('/exist', $key), 2);
        $rez = 'OK' == $rez;
        //return [$rez, $valRaw];
        $val = $rez ? $this->parseGet($valRaw) : NULL;
        return $rez;
    }
    
    /**
      *  remove key from db
      */
    public function del($key) {
        return 'OK' == $this->httpPost('/del', $key);
    }
    
    /**
      *  Multi remove keys from db
      */
    public function mdel($keys) {
        return $this->httpPost('/mdel', implode("\n", $keys) );
    }
    
    /**
      *  incriment value by key
      */
    public function incr($key, $value = NULL) {
        return 'OK' == (
                    $value ?
                    $this->httpPost('/incr', "$key&$value" ) :
                    $this->httpPost('/incr', $key )
                );
    }
    
    /**
      *  backup database
      */
    public function backup() {
        return $this->httpPost('/backup');
        return 'OK' == $this->httpPost('/backup');
    }
    
    
}

