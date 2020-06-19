" Matching erroneous syntax
syn match key_containing_spaces '^[[:blank:]]*[0-9a-zA-Z_-]\+[[:blank:]]\+[^=[:blank:]]\+'
syn match invalid_char '.'

" Keywords
syn keyword quicktest_aliases contained FILE_TO_OPEN INSTANCE INSTANCE_PATH INSTANCES_DIR
syn keyword quicktest_aliases contained OUTPUT_DIR OUTPUT_PATH QUICKTEST_PATH
syn keyword quicktest_funcs contained BASH VAR

" Keys
syn match equal_to contained '='
syn match key contained '[0-9a-zA-Z_-]'
syn match key_and_equal_to '^[[:blank:]]*[0-9a-zA-Z_-]\+[[:blank:]]*=' contains=equal_to,key

" Values of keys
syn region quicktest_preproc_block start="\[" end="]" contained fold contains=quicktest_aliases,quicktest_funcs
syn match quicktest_value_block '^\t.*$' fold contains=quicktest_preproc_block
syn match quicktest_value_block '^+.*$' fold contains=quicktest_preproc_block

" uncomment the following 2 lines to show tabs
"set list
"set listchars=tab:<->

let b:current_syntax = "qtc"

hi def link quicktest_aliases      Keyword
hi def link quicktest_funcs        Function
hi def link key                    Underlined
hi def link key_containing_spaces  Error
hi def link invalid_char           Error
hi def link equal_to               Type
