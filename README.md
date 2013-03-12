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
```
phpize
./configure --with-php-config=/usr/local/php/bin/php-config
make && make install
```
Then add
```
extension="array_ext.so"
```
to your php.ini

#Usage

###array_split()
```php
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
```

###array_column()
```php
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
```

###array_trim()
```php
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
```

###array_columnkey()
```php
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
```

###array_delete()
```php
$res=array_delete(['apple', 'banana', 'pear', 'apple'], 'apple');
print_r($res);
/*outputs:
Array
(
    [1] => banana
    [2] => pear
)
*/
```

###array_udelete()
```php
$res=array_udelete(['apple', 'banana', 'pear', 'apple'], function($val){
		return $val=='banana';
	});
print_r($res);
/*outputs:
Array
(
    [0] => apple
    [2] => pear
    [3] => apple
)
*/
```

###array_keys_filter()
```php
$arr=['name'=>'bar', 'age'=>23, 'grade'=>4, 'money'=>1234, 'sex'=>true];
$res=array_keys_filter($arr, ['age', 'money', 'sex']);
print_r($res);
/*outputs:
Array
(
    [age] => 23
    [money] => 1234
    [sex] => 1
)
*/
```

###
array_update()
```php
$arr=['name'=>'bar', 'age'=>23, 'grade'=>4, 'money'=>1234, 'sex'=>true];
$res=array_update($arr, ['age'=>15, 'money'=>100, 'sex'=>false]);
print_r($res);
/*outputs:
Array
(
    [name] => bar
    [age] => 15
    [grade] => 4
    [money] => 100
    [sex] => 
)
*/
```
