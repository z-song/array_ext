/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2012 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: merlinal                                                     |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_array_ext.h"

/* If you declare any globals in php_array_ext.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(array_ext)
*/

/* True global resources - no need for thread safety here */
static int le_array_ext;

/* {{{ arginfo */
ZEND_BEGIN_ARG_INFO_EX(arginfo_array_split, 0, 0, 2)
	ZEND_ARG_INFO(0, arg)
	ZEND_ARG_INFO(0, size)
	ZEND_ARG_INFO(0, preserve_keys)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_column, 0, 0, 1)
	ZEND_ARG_INFO(0, arr)
	ZEND_ARG_INFO(0, keys)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_keys_filter, 0, 0, 1)
	ZEND_ARG_INFO(0, arr)
	ZEND_ARG_INFO(0, keys)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_delete, 0, 0, 1)
	ZEND_ARG_INFO(0, arr)
	ZEND_ARG_INFO(0, arg)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_udelete, 0, 0, 1)
	ZEND_ARG_INFO(0, arg)
	ZEND_ARG_INFO(0, callback)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_array_update, 0, 0, 1)
	ZEND_ARG_INFO(0, arr1)
	ZEND_ARG_INFO(0, arr2)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ array_ext_functions[]
 */
const zend_function_entry array_ext_functions[] = {
	PHP_FE(array_split,		arginfo_array_split)
	PHP_FE(array_column,		arginfo_array_column)
	PHP_FE(array_keys_filter,	arginfo_array_keys_filter)
	PHP_FE(array_delete,		arginfo_array_delete)
	PHP_FE(array_udelete,		arginfo_array_udelete)
	PHP_FE(array_update,		arginfo_array_update)
	
	PHP_FE_END
};
/* }}} */

/* {{{ array_ext_module_entry
 */
zend_module_entry array_ext_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"array_ext",
	array_ext_functions,
	PHP_MINIT(array_ext),
	PHP_MSHUTDOWN(array_ext),
	PHP_RINIT(array_ext),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(array_ext),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(array_ext),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_ARRAY_EXT
ZEND_GET_MODULE(array_ext)
#endif

PHP_MINIT_FUNCTION(array_ext)
{
	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(array_ext)
{
	return SUCCESS;
}

PHP_RINIT_FUNCTION(array_ext)
{
	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(array_ext)
{
	return SUCCESS;
}

PHP_MINFO_FUNCTION(array_ext)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "array_ext support", "enabled");
	php_info_print_table_end();
}

PHP_FUNCTION(array_split)
{
	zval *input,**entry;
	long nums,current=0;
	int num_in,size;
	HashPosition pos;
	zval *piece = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "al", &input,&nums) == FAILURE) {
		return;
	}
	
	num_in = zend_hash_num_elements(Z_ARRVAL_P(input));
	size=(int)((num_in-1)/nums+1);
	array_init_size(return_value, nums);
	
	zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(input), &pos);
	while (zend_hash_get_current_data_ex(Z_ARRVAL_P(input), (void**)&entry, &pos) == SUCCESS) {
		if(!piece){
			MAKE_STD_ZVAL(piece);
			array_init_size(piece, size);
		}
		zval_add_ref(entry);
		add_next_index_zval(piece, *entry);
	
		if (!(++current % size)) {
			add_next_index_zval(return_value, piece);
			piece = NULL;
		}
		
		zend_hash_move_forward_ex(Z_ARRVAL_P(input), &pos);
	}
	if (piece) {
		add_next_index_zval(return_value, piece);
	}
}


PHP_FUNCTION(array_delete)
{
	zval *array,*value, **entry ,result;
	char	 *string_key;
	uint	  string_key_len;
	ulong	  num_key;
	
	int (*is_equal_func)(zval *, zval *, zval * TSRMLS_DC) = is_equal_function;
	
	HashTable *arr_hash;
	HashPosition pointer;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "az", &array, &value) == FAILURE) {
		return;
	}
	
	arr_hash = Z_ARRVAL_P(array);
	array_init(return_value);
	
	for(zend_hash_internal_pointer_reset_ex(arr_hash, &pointer);
		zend_hash_get_current_data_ex(arr_hash, (void **)&entry, &pointer) == SUCCESS;
		zend_hash_move_forward_ex(arr_hash, &pointer)
	){
		is_equal_func(&result, value, *entry TSRMLS_CC);
		if (!Z_LVAL(result)) {
			switch (zend_hash_get_current_key_ex(arr_hash, &string_key, &string_key_len, &num_key, 0, &pointer)) {
				case HASH_KEY_IS_STRING:
					zend_hash_update(Z_ARRVAL_P(return_value), string_key, string_key_len, entry, sizeof(zval *), NULL);
					break;

				case HASH_KEY_IS_LONG:
					zend_hash_index_update(Z_ARRVAL_P(return_value), num_key, entry, sizeof(zval *), NULL);
			}
		}
	}
}


PHP_FUNCTION(array_udelete)
{
	zval *array;
	zval **operand;
	zval **args[1];
	zval *retval = NULL;
	zend_bool have_callback = 0;
	char *string_key;
	zend_fcall_info fci = empty_fcall_info;
	zend_fcall_info_cache fci_cache = empty_fcall_info_cache;
	uint string_key_len;
	ulong num_key;
	HashPosition pos;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "a|f", &array, &fci, &fci_cache) == FAILURE) {
		return;
	}

	array_init(return_value);
	if (zend_hash_num_elements(Z_ARRVAL_P(array)) == 0) {
		return;
	}

	if (ZEND_NUM_ARGS() > 1) {
		have_callback = 1;
		fci.no_separation = 0;
		fci.retval_ptr_ptr = &retval;
		fci.param_count = 1;
	}

	for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(array), &pos);
		zend_hash_get_current_data_ex(Z_ARRVAL_P(array), (void **)&operand, &pos) == SUCCESS;
		zend_hash_move_forward_ex(Z_ARRVAL_P(array), &pos)
	) {
		if (have_callback) {
			args[0] = operand;
			fci.params = args;

			if (zend_call_function(&fci, &fci_cache TSRMLS_CC) == SUCCESS && retval) {
				if (!zend_is_true(retval)) {
					zval_ptr_dtor(&retval);
				} else {
					zval_ptr_dtor(&retval);
					continue;
				}
			} else {
				php_error_docref(NULL TSRMLS_CC, E_WARNING, "An error occurred while invoking the filter callback");
				return;
			}
		} else if (!zend_is_true(*operand)) {
			continue;
		}

		zval_add_ref(operand);
		switch (zend_hash_get_current_key_ex(Z_ARRVAL_P(array), &string_key, &string_key_len, &num_key, 0, &pos)) {
			case HASH_KEY_IS_STRING:
				zend_hash_update(Z_ARRVAL_P(return_value), string_key, string_key_len, operand, sizeof(zval *), NULL);
				break;

			case HASH_KEY_IS_LONG:
				zend_hash_index_update(Z_ARRVAL_P(return_value), num_key, operand, sizeof(zval *), NULL);
				break;
		}
	}
}


PHP_FUNCTION(array_keys_filter)
{
	zval *array, *keys, **keys_item,**zvalue;
	char *key=NULL;
	int key_len = 0,size=0;	
	long index=0;
	
	HashTable *keys_hash;
	HashPosition keys_pointer;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "aa", &array, &keys) == FAILURE) {
		return;
	}
	
	keys_hash = Z_ARRVAL_P(keys);
	array_init(return_value);
	
	zend_hash_internal_pointer_reset_ex(keys_hash, &keys_pointer);
	while (zend_hash_get_current_data_ex(keys_hash, (void **)&keys_item, &keys_pointer) == SUCCESS) {
		if(Z_TYPE_PP(keys_item)==IS_LONG){
			index = Z_LVAL_PP(keys_item);
			if(zend_hash_index_find(Z_ARRVAL_P(array), index, (void**)&zvalue)==SUCCESS){
				Z_ADDREF_PP(zvalue);
				add_next_index_zval(return_value, *zvalue);
			}
		}else if(Z_TYPE_PP(keys_item)==IS_STRING){
			key=Z_STRVAL_PP(keys_item);
			key_len=Z_STRLEN_PP(keys_item);
			if(zend_hash_find(Z_ARRVAL_P(array), key, key_len + 1, (void**)&zvalue) == SUCCESS){
				Z_ADDREF_PP(zvalue);
				add_next_index_zval(return_value, *zvalue);
			}
		}
		zend_hash_move_forward_ex(keys_hash, &keys_pointer);
	}
}


PHP_FUNCTION(array_update)
{
	zval *array, *update, **entry, **zvalue;
	char	 *string_key;
	uint	  string_key_len;
	ulong	  num_key;
	
	HashTable *arr_hash;
	HashPosition pos;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "aa", &array, &update) == FAILURE) {
		return;
	}
	
	arr_hash = Z_ARRVAL_P(array);
	array_init(return_value);
	
	for(zend_hash_internal_pointer_reset_ex(arr_hash, &pos);
		zend_hash_get_current_data_ex(arr_hash, (void **)&entry, &pos) == SUCCESS;
		zend_hash_move_forward_ex(arr_hash, &pos)
	){
		Z_ADDREF_PP(entry);
		switch (zend_hash_get_current_key_ex(arr_hash, &string_key, &string_key_len, &num_key, 0, &pos)) {
			case HASH_KEY_IS_STRING:
				if(zend_hash_find(Z_ARRVAL_P(update), string_key, string_key_len, (void**)&zvalue) == SUCCESS){
					Z_ADDREF_PP(zvalue);
					zend_hash_update(Z_ARRVAL_P(return_value), string_key, string_key_len, zvalue, sizeof(zval *), NULL);
				}else{
					zend_hash_update(Z_ARRVAL_P(return_value), string_key, string_key_len, entry, sizeof(zval *), NULL);
				}
				break;
			case HASH_KEY_IS_LONG:
				if(zend_hash_index_find(Z_ARRVAL_P(update), num_key, (void**)&zvalue)==SUCCESS){
					Z_ADDREF_PP(zvalue);
					zend_hash_index_update(Z_ARRVAL_P(return_value), num_key, zvalue, sizeof(zval *), NULL);
				}else{
					zend_hash_index_update(Z_ARRVAL_P(return_value), num_key, entry, sizeof(zval *), NULL);
				}
		}
	}
}

PHP_FUNCTION(array_column)
{
	zval *zarray, *zoffset, **data, **zvalue,**entry;
	HashTable *arr_hash;
	HashPosition pointer,pos;
	long index = 0;
	char *key = NULL;
	int key_len = 0,size;
	zend_bool columns = 0;
	zval *piece = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "az", &zarray, &zoffset) == FAILURE) {
		return;
	}

	arr_hash = Z_ARRVAL_P(zarray);
	array_init(return_value);
	
	switch (Z_TYPE_P(zoffset)) {
		case IS_NULL:
			index = 0;
			break;
		case IS_LONG:
			index = Z_LVAL_P(zoffset);
			break;
		case IS_STRING:
			key = Z_STRVAL_P(zoffset);
			key_len = Z_STRLEN_P(zoffset);
			break;
		case IS_OBJECT:
			convert_to_string(zoffset);
			key = Z_STRVAL_P(zoffset);
			key_len = Z_STRLEN_P(zoffset);
			break;
		case IS_ARRAY:
			columns = 1;
			size = zend_hash_num_elements(Z_ARRVAL_P(zoffset));
			break;
		default:
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "The key should be either a string or an integer");
			RETURN_FALSE;
	}
	
	zend_hash_internal_pointer_reset_ex(arr_hash, &pointer);
	while (zend_hash_get_current_data_ex(arr_hash, (void **)&data, &pointer) == SUCCESS) {
		if (Z_TYPE_PP(data) == IS_ARRAY) {
			if(columns){
				if(!piece){
					MAKE_STD_ZVAL(piece);
					array_init_size(piece, size);
				}
				zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(zoffset), &pos);
				while (zend_hash_get_current_data_ex(Z_ARRVAL_P(zoffset), (void **)&entry, &pos) == SUCCESS) {
					key = Z_STRVAL_PP(entry);
					key_len = Z_STRLEN_PP(entry);
					if (key && zend_hash_find(Z_ARRVAL_PP(data), key, key_len + 1, (void**)&zvalue) == FAILURE) {
						continue;
					}
					
					Z_ADDREF_PP(zvalue);
					add_next_index_zval(piece, *zvalue);
					zend_hash_move_forward_ex(Z_ARRVAL_P(zoffset), &pos);
				}
				
				add_next_index_zval(return_value, piece);
				piece = NULL;
			}else{
				if (key && zend_hash_find(Z_ARRVAL_PP(data), key, key_len + 1, (void**)&zvalue) == FAILURE) {
					continue;
				} else if (!key && zend_hash_index_find(Z_ARRVAL_PP(data), index, (void**)&zvalue) == FAILURE) {
					continue;
				}

				Z_ADDREF_PP(zvalue);
				add_next_index_zval(return_value, *zvalue);
			}
		}
		zend_hash_move_forward_ex(arr_hash, &pointer);
	}
}

