#!/usr/bin/php
<?php

require "Rocks.php";



$db = new Rocks\Client();

# Test get
var_export($db->get('key1'));
var_export($db->get('noexist'));
var_export($db->get(''));
echo PHP_EOL, PHP_EOL, PHP_EOL;
/**/
//exit;

# Test mget
/*
var_export($db->mget(['key1','key2','key3']));
echo PHP_EOL, PHP_EOL, PHP_EOL;
var_export($db->mget(['key1','k2','kw3', 'noexist']));
echo PHP_EOL, PHP_EOL, PHP_EOL;
var_export($db->mget(['key1']));
echo PHP_EOL, PHP_EOL, PHP_EOL;
var_export($db->mget([]));
exit;
*/

# Test set
//var_export($db->set('key1', 'val1'));
//var_export($db->set('key2', 'val2'));var_export($db->set('key3', 'val3'));
//echo PHP_EOL, PHP_EOL, PHP_EOL;

/*
echo PHP_EOL, PHP_EOL, PHP_EOL;
$v = 'val'.mt_rand(0,500);
var_export([$db->get('asd'), $db->set('asd',  $v), $v]);
echo PHP_EOL, PHP_EOL, PHP_EOL;
$k = 'rnd' . mt_rand(1,10);
$v = 'v:' . md5(mt_rand(0,500));
var_export($db->set($k, $v));
echo PHP_EOL, PHP_EOL;
var_export($db->get($k));

echo PHP_EOL, PHP_EOL, PHP_EOL;
*/

# Test mset
/*
var_export($db->mget(['mskey1','mskey2','mskey3']));
echo PHP_EOL, PHP_EOL, PHP_EOL;
var_export($db->mset(['mskey1' => 'v1' . mt_rand(0,500), 'mskey2' => 'v2' . mt_rand(0,500), 'mskey3' => 'v3' . mt_rand(0,500)]));
echo PHP_EOL, PHP_EOL, PHP_EOL;
var_export($db->mget(['mskey1','mskey2','mskey3']));
*/


# Test keyExist
/*
var_export($db->get('key1'));
echo PHP_EOL;
var_export([$db->keyExist('key1', $value), $value]);
echo PHP_EOL;
var_export([$db->keyExist('key2', $value), $value]);
echo PHP_EOL;
var_export([$db->keyExist('noexist', $value), $value]);
echo PHP_EOL;
var_export([$db->keyExist('', $value), $value]);
echo PHP_EOL;
*/

# Test del
/*
var_export($db->set('test1', 'val1'));
echo PHP_EOL;
var_export($db->get('test1'));
echo PHP_EOL;
var_export($db->del('test1'));
echo PHP_EOL;
var_export($db->get('test1'));
echo PHP_EOL;
*/

# Test mdel
///*
var_export($db->mget(['mdkey1','mdkey2','mdkey3']));
echo PHP_EOL, PHP_EOL;

var_export($db->mset(['mdkey1' => 'v1' . mt_rand(0,500), 'mdkey2' => 'v2' . mt_rand(0,500), 'mdkey3' => 'v3' . mt_rand(0,500)]));
echo PHP_EOL, PHP_EOL;

var_export($db->mget(['mdkey1','mdkey2','mdkey3']));
echo PHP_EOL, PHP_EOL;

var_export($db->mdel(['mdkey1','mdkey2','mdkey3']));
echo PHP_EOL, PHP_EOL;

var_export($db->mget(['mdkey1','mdkey2','mdkey3']));
echo PHP_EOL, PHP_EOL;
//*/

# Test incr
/*
var_export($db->get('incrtest'));
echo PHP_EOL, PHP_EOL;

var_export($db->incr('incrtest'));
echo PHP_EOL, PHP_EOL;

var_export($db->get('incrtest'));
echo PHP_EOL, PHP_EOL;

var_export($db->incr('incrtest', 7));
echo PHP_EOL, PHP_EOL;

var_export($db->get('incrtest'));
echo PHP_EOL, PHP_EOL;

var_export($db->incr('incrtest', -2));
echo PHP_EOL, PHP_EOL;

var_export($db->get('incrtest'));
echo PHP_EOL, PHP_EOL;
*/