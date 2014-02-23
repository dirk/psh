## psh: the programmer's shell

psh is heavily inspired by the venerable csh, tcsh, and fish shells. It also draws from the programming languages C, Perl, Ruby, and JavaScript (but really only the JSON part of JavaScript).

### Syntax

```shell
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

#### Keywords

There are two options for keywords:

1. Have a set of well-defined, well-known keywords internal to the shell. Everything else is external.
2. Have a special prefix to clearly denote a keyword operator, eg. `:`.

With option 1 things would look like:

```ruby
if(test -d .)
  echo "It exists!"
end
```

With option 2:
```ruby
:if(test -d .)
  echo "It exists!"
:end
```

### Implementation

Need to define an intelligent parsing architecture for handling parsing, hinting, and evaluating input.

