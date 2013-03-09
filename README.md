#array_ext
一些php数组操作函数
   * [array_split](#array_split)
   * [array_column](#array_column)
   * [array_trim](#array_trim)
   * [array_columnkey](#array_columnkey)
   * [array_delete](#array_delete)
   * [array_udelete](#array_udelete)
   * [array_keys_filter](#array_keys_filter)
   * [array_update](#array_update)


# Installing/Configuring

~~~
phpize
./configure --with-php-config=/usr/local/php/bin/php-config
make && make install
~~~
Then add
~~~
extension="array_ext.so"
~~~
to your php.ini

#Usage

###array_split()
~~~
$arr=[1,3,4,5,6,34,67];
$res=array_split($arr, 3);
print_r($res);
/*outputs:
Array
(
    [0] => Array
        (
            [0] => 1
            [1] => 3
            [2] => 4
        )
    [1] => Array
        (
            [0] => 5
            [1] => 6
            [2] => 34
        )
    [2] => Array
        (
            [0] => 67
        )
)
*/
~~~

###array_column()
~~~
$arr=[
	['name'=>'bar','age'=>34],
	['name'=>'foo','age'=>17],
	['name'=>'tar','age'=>43],
	['name'=>'gas','age'=>23]
];
$res=array_column($arr, 'age');
print_r($res);
/*outputs:
Array
(
    [0] => 34
    [1] => 17
    [2] => 43
    [3] => 23
)
*/
~~~

###array_trim()
~~~
$res=array_trim([[3,5],3,5,7,5,6,7,8,[3,5]],[3,5]);
print_r($res);
/*outputs:
Array
(
    [1] => 3
    [2] => 5
    [3] => 7
    [4] => 5
    [5] => 6
    [6] => 7
    [7] => 8
)
*/
~~~

###array_columnkey()
~~~
$arr=[
	['name'=>'hello','age'=>12],
	['name'=>'hello','age'=>13],
	['name'=>'hello','age'=>14],
	['name'=>'hello','age'=>15],
];
$res=array_columnkey($arr,'age');
print_r($res);
/*outputs:
Array
(
    [12] => Array
        (
            [name] => hello
            [age] => 12
        )
    [13] => Array
        (
            [name] => hello
            [age] => 13
        )
    [14] => Array
        (
            [name] => hello
            [age] => 14
        )
    [15] => Array
        (
            [name] => hello
            [age] => 15
        )
*/
~~~

###array_delete()
~~~
$res=array_delete(['apple', 'banana', 'pear', 'apple'], 'apple');
print_r($res);
/*outputs:
Array
(
    [1] => banana
    [2] => pear
)
*/
~~~
