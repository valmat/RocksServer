<?php
/**
 *  API for RocksServer
 *  Response container
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */
namespace RocksServer;

class Response{
    
    /**
      *   socket
      */
    private $_sock = NULL;
    
    function __construct($sock) {
        $this->_sock = $sock;
    }
    
    function __destruct() {
        $this->close();
    }
    
    /**
      *  Close socket
      */
    public function close() {
        if(!is_null($this->_sock)) {
            fclose($this->_sock);
            $this->_sock = NULL;
        }
    }
    
    /**
      *  Raw data of response
      *  @return string
      */
    public function raw(){
        $rez = '';
        while (!feof($this->_sock)) {
            $s =  fgets($this->_sock);
            $rez .= $s;
        }
        $this->close();
        return $rez;
    }
    
    /**
      *  Cast response to bool
      *  @return bool response is OK
      */
    public function isOk(){
        if(!feof($this->_sock)) {
            $r = fgets($this->_sock);
            // $r == 'OK'
            return  (strlen($r) > 1 && 'O' == $r{0} && 'K' == $r{1}); 
        }
        return false;
    }
    
    /**
      *  Get single value of response
      *  @return string
      */
    public function getValue() {
        if(feof($this->_sock)) {
            return NULL;
        }
        $rezlen = (int)fgets($this->_sock);
        if( $rezlen < 0 || feof($this->_sock) ) {
            return NULL;
        }
        $rez = fgets($this->_sock);
        
        return (strlen($rez) > $rezlen) ? substr($rez, 0, $rezlen) : $rez;
    }
    
    /**
      *  Get multi-value iterator of response
      *  @return MgetIterator
      */
    public function getMultiValue() {
        return new MgetIterator($this);
    }
    
    /**
      *  Check if socket is valid
      *  @return bool
      */
    public function isValid() {
        return !feof($this->_sock);
    }
    
    /**
      *  Read data from socket
      *  @return string
      */
    public function read($len = 0) {
        if(!$len) return fgets($this->_sock);
        
        $rez = fgets($this->_sock, $len);
        $rlen = strlen($rez);
        
        while($rlen < $len-1 && !feof($this->_sock)) {
            $rez .= fgets($this->_sock, $len-$rlen);
            $rlen = strlen($rez);
        }
        return $rez;
    }
    
    /**
      *  Cast to string
      */
    public function __toString(){
        return $this->raw();
    }
    
}


