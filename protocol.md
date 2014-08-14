## RocksServer Protocol

###Get
Get value by key

Request:
```
GET /get?key HTTP/1.1
```
Reply:

```
<value_len>\n
<value>
```
If the key does not exist in the database, the reply will be:
```
-1\n
```

###Multi get
Get values by keys

Request:
```
GET /mget?key1&key2&key3 HTTP/1.1
```
Reply:

```
<value1_len>\n
<value1>\n
<value2_len>\n
<value2>\n
<value3_len>\n
<value3>\n
```
If the keyN does not exist, then `<valueN_len> = -1`, `<valueN>` -- is empty


###set
Set value by key

Request:
```
POST /set HTTP/1.1
Content-Length: <content_length>\r\n
\r\n
<key>\n
<value_len>\n
<value>
```
Reply:
```
OK
```
If successful.
And reply:
```
FAIL
```
on fail.


###Multi set
Set values by keys

Request:
```
POST /mset HTTP/1.1
Content-Length: <content_length>\r\n
\r\n
<key1>\n
<value1_len>\n
<value1>
<key2>\n
<value2_len>\n
<value2>
<key3>\n
<value3_len>\n
<value3>
```
Reply:
```
OK
```
If successful.
And reply:
```
FAIL
```
on fail.

###delete key
Delete key from DB

Request:
```
POST /del HTTP/1.1
Content-Length: <content_length>\r\n
\r\n
<key>
```
Reply:
```
OK
```
If successful.
And reply:
```
FAIL
```
on fail.


###Multi delete keys
Set values by keys

Request:
```
POST /mdel HTTP/1.1
Content-Length: <content_length>\r\n
\r\n
<key1>\n
<key2>\n
<key3>
```
Reply:
```
OK
```
If successful.
And reply:
```
FAIL
```
on fail.

###Key exist?
Check key existing

Request:
```
GET /exist?key HTTP/1.1
```
Reply:

```
OK
<value_len>\n
<value>
```
if the key exists and is able to retrieve its value.

Reply:
```
OK
-1\n
```
if the key exists but was not able to retrieve its value.

Reply:
```
FAIL
```
if the key does not exist in the database.

###Imcrement
Imcrement value by key

Request:
```
GET /incr?key HTTP/1.1
```
to increase the value on 1.

Or request:
```
GET /incr?key&value HTTP/1.1
```
to increase the value on `<value>`.

Reply:
```
OK
```
If successful.
And reply:
```
FAIL
```
on fail.

```