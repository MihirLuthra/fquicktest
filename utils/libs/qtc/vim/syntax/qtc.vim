syn keyword quicktest_vars contained FILE_TO_OPEN INSTANCE INSTANCE_PATH
syn keyword quicktest_vars contained OUTPUT_DIR OUTPUT_PATH QUICKTEST_PATH

syn keyword quicktest_funcs contained BASH STRIP_EXTENSION


syn match equal_to contained '='
syn match key '[[:blank:]]*[0-9a-zA-Z_-]\+[[:blank:]]*=[[:blank:]]*' contains=equal_to,pound
syn match trailing_spaces_after_equal_to '[[:blank:]]*[0-9a-zA-Z_-]\+[[:blank:]]*=[[:blank:]]*[^[:blank:]{]\+'
syn match key_containing_spaces '^[[:blank:]]*[0-9a-zA-Z_-]\+[[:blank:]]\+[^=[:blank:]]\+'
syn match invalid_char '[^=0-9a-zA-Z_{}[:blank:]-]'


syn region quicktest_vars_block start="\[" end="]" contained fold contains=quicktest_vars,quicktest_funcs

syn region sstring start="'" end="'" contained contains=quicktest_vars,quicktest_funcs
syn region dstring start='"' end='"' contained contains=quicktest_vars,quicktest_funcs

syn region quicktest_value_block start="{" end="}" fold contains=quicktest_vars_block,dstring,sstring

let b:current_syntax = "quicktest"

hi def link quicktest_vars                  Keyword
hi def link quicktest_funcs                 PreProc
hi def link key                             Type
hi def link trailing_spaces_after_equal_to  Error
hi def link key_containing_spaces           Error
hi def link invalid_char                    Error
hi def link equal_to                        Operator
hi def link dstring                         String
hi def link sstring                         String
