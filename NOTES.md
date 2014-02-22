## psh: the programmer's shell

### Syntax

```bash
# All variables prefixed with $
# Variables starting with an upper case character are automatically
# global environmental variables (and all subsequent characters must be
# an uppercase letter or an underscore).
$local
$ENV
$PATH
# Variable scoping *shouldn't* be hard. Let's maybe have 'global' and 'local'
# keywords to adjust scoping in functions/subroutines/etc.

# Variable assignment options
# 1. classical command-style
set $name "value"
# 2. maybe do something more programmatic (bash-style)
$name = "value"
$PATH = "/usr/local/bin:$PATH" # This is the same as `export $PATH=/usr/local/bin:$PATH` in bash
```

### Implementation

Need to define an intelligent parsing architecture for handling parsing, hinting, and evaluating input.

