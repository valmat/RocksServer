<?php
/**
 *  API for RocksServer
 *  Multi get result iterator
 *
 *  @author valmat <ufabiz@gmail.com>
 *  @github https://github.com/valmat/rocksserver
 */
namespace RocksServer;

class MgetIterator implements \Iterator {
    
    /**
      *   Response object
      */
    private $_resp = NULL;
    
    /**
      *   Current key and value
      */
    private $_key;
    private $_val;
    
    /**
      *   Iterator is valid
      */
    private $_valid;
    
    
    function __construct(Response $resp) {
        $this->_resp = $resp;
    }
    
    /**
      *   Return the current element
      */
    function current() {
        return $this->_val;
    }

    /**
      *   Return the key of the current element
      */
    function key() {
        return $this->_key;
    }
    
    /**
      *  Rewind the Iterator to the first element 
      */
    function rewind() {
        $this->next();
    }

    /**
      *   Move forward to next element
      */
    function next() {
        if( !($this->_valid = $this->_resp->isValid() ) ){
            return;
        }
        
        $this->_key = substr($this->_resp->read(), 0, -1);
        $val_len = (int)$this->_resp->read();
        if($val_len < 0) {
            $this->_val = NULL;
            return;
        }
        $this->_val = $val_len ? $this->_resp->read($val_len+1) : '';
        
        // skip '\n'
        $this->_resp->read();
    }

    /**
      *   Checks if current position is valid
      */
    function valid() {
        return $this->_valid;
    }
    
    /**
      *  Show debug info
      */
    public function show() {
        foreach($this as $key => $value) {
            echo "$key\t=>\t";
            var_dump($value);
        }
    }
    
    

    
}


