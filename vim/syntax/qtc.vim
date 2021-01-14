" Matching erroneous syntax
syn match key_containing_spaces '^[[:blank:]]*[0-9a-zA-Z_-]\+[[:blank:]]\+[^=[:blank:]]\+'
syn match invalid_char '.'

" Keywords
syn keyword clium_aliases contained FILE_TO_OPEN INSTANCE INSTANCE_BASENAME INSTANCE_PATH INSTANCES_DIR
syn keyword clium_aliases contained OUTPUT_DIR D_OUTPUT_DIR OUTPUT_DIR CLIUM_PATH
syn keyword clium_funcs contained BASH VAR

" Keys
syn match equal_to contained '='
syn match key contained '[0-9a-zA-Z_-]'
syn match key_and_equal_to '^[[:blank:]]*[0-9a-zA-Z_-]\+[[:blank:]]*=' contains=equal_to,key

" Values of keys
syn region clium_preproc_block start="\[" end="]" contained fold contains=clium_aliases,clium_funcs
syn match clium_value_block '^\t.*$' fold contains=clium_preproc_block
syn match clium_value_block '^+.*$' fold contains=clium_preproc_block

" uncomment the following 2 lines to show tabs
"set list
"set listchars=tab:<->

let b:current_syntax = "qtc"

hi def link clium_aliases          Keyword
hi def link clium_funcs            Function
hi def link key                    Underlined
hi def link key_containing_spaces  Error
hi def link invalid_char           Error
hi def link equal_to               Type
