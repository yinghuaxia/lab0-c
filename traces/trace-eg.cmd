# Demonstration of queue testing framework
# Use help command to see list of commands and options
# Initial queue is NULL.
show
option fail 0
option malloc 0
new
ih a
ih r
ih b
sort
new
ih m
ih n
ih a
sort
new
ih r
ih c
ih z
sort
# merge
merge
reverse
rh z
rh r
rh r
rh n
quit
