## RocksServer Protocol

### Get
Get value by key

__Request__:
```
GET /get?key HTTP/1.1
```
__Response__:
```
<value_len>\n
<value>
```
If the key does not exist in the database, the reply will be:
```
-1\n
```

### Multi get
Get values by keys

__Request__:
```
GET /mget?key1&key2&key3 HTTP/1.1
```
__Response__:

```
<key1>\n
<value1_len>\n
<value1>\n
<key2>\n
<value2_len>\n
<value2>\n
<key3>\n
<value3_len>\n
<value3>\n
```
If the keyN does not exist, then `<valueN_len> = -1`, `<valueN>` -- is empty


### Set
Set value by key

__Request__:
```
POST /set HTTP/1.1
Content-Length: <content_length>\r\n
\r\n
<key>\n
<value_len>\n
<value>
```
__Response__:
```
OK
```
If successful.
And reply:
```
FAIL
```
on fail.


### Multi set
Set values by keys

__Request__:
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
__Response__:
```
OK
```
If successful.
And reply:
```
FAIL
```
on fail.

### Delete key
Delete key from DB

__Request__:
```
POST /del HTTP/1.1
Content-Length: <content_length>\r\n
\r\n
<key>
```
__Response__:
```
OK
```
If successful.
And reply:
```
FAIL
```
on fail.


### Multi delete keys
Delete keys from DB

__Request__:
```
POST /mdel HTTP/1.1
Content-Length: <content_length>\r\n
\r\n
<key1>\n
<key2>\n
<key3>
```
__Response__:
```
OK
```
If successful.
And reply:
```
FAIL
```
on fail.

### Check if key exists

__Request__:
```
GET /exist?key HTTP/1.1
```
__Response__:

```
OK
<value_len>\n
<value>
```
if the key exists and is able to retrieve its value.

__Response__:
```
OK
-1\n
```
if the key exists but was not able to retrieve its value.

__Response__:
```
FAIL
```
if the key does not exist in the database.

### Imcrement
Imcrement value by key

__Request__:
```
POST /incr HTTP/1.1
Content-Length: <content_length>\r\n
\r\n
<key>
```
to increase the value on 1.

Or __request__:
```
POST /incr HTTP/1.1
Content-Length: <content_length>\r\n
\r\n
<key>\n
<value>
```
to increase the value on `<value>`.

__Response__:
```
OK
```
If successful.
And reply:
```
FAIL
```
on fail.

### Get all keys and values

__Request__:
```
POST /tail
```

__Response__: the same as [multi get](https://github.com/valmat/RocksServer/blob/master/protocol.md#multi-get)


### Get keys and values by key-prefix

__Request__:
```
GET /prefit?<prefix>
```

__Response__: the same as [multi get](https://github.com/valmat/RocksServer/blob/master/protocol.md#multi-get)


### Get keys and values by seek key-prefix

__Request__:
```
GET /seekprev?<key-prefix-start>[&<starts-with>]
```
```
GET /seekprev-range?<key-prefix-start>&<key-prefix-end>[&<starts-with>]
```
```
GET /seeknext?<key-prefix-start>[&<starts-with>]
```
```
GET /seeknext-range?<key-prefix-start>&<key-prefix-end>[&<starts-with>]
```

`seekprev` and `seekprev-range` seeks for first key less then `<key-prefix-start>`.

`seeknext` and `seeknext-range` seeks for first key equal or more then `<key-prefix-start>`.


`seekprev-range` and `seeknext-range` iterate while key less or equal `<key-prefix-end>`

If optional parameter `<starts-with>` given iterate, while key starts with prefix `<starts-with>`


__Response__: the same as [multi get](https://github.com/valmat/RocksServer/blob/master/protocol.md#multi-get)


### Backup
Backup database

__Request__:
```
POST /backup
```

__Response__:
```
OK
```
If successful.
Or reply:
```
<Error message>
```
on fail.


### Stats
Usage statistics

__Request__:
```
GET /stats
```

__Response__: stats output
