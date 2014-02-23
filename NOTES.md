## psh: the programmer's shell

psh is heavily inspired by the venerable csh, tcsh, and fish shells. It also draws from the programming languages C, Perl, Ruby, and JavaScript (but really only the JSON part of JavaScript).

### Syntax

#### Variables

All variables will be prefixed with `$`. Variables starting with an upper case character are automatically global environmental variables (and all subsequent characters must be an uppercase letter or an underscore).

```shell
$local
$ENV
$PATH

# Variable assignment options
# 1. classical command-style
:set $name "value"
# 2. maybe do something more programmatic (bash-style)
$name = "value"
$PATH = "/usr/local/bin:$PATH" # This is the same as `export $PATH=/usr/local/bin:$PATH` in bash
```

Also, variable scoping *shouldn't* be hard. Let's maybe have 'global' and 'local' keywords to adjust scoping in functions/subroutines/etc.

#### Keywords

Keywords will be prefixed with a colon character to distinguish them from external stuff (files, programs, etc.)

```shell
:if(test -d .)
  echo "It exists!"
:end
```

### Implementation

Need to define an intelligent parsing architecture for handling parsing, hinting, and evaluating input.

