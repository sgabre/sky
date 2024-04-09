
# git configuration
git config --global user.name "<username>"
git config --global user.email "<email address>
git config --global init.default branch main

# git create branch

# git commit

# git push

# git pull



# How to build 

1. build the software system in debug mode

cmake -DTARGET_GROUP=debug .. 
make

2. build the software system in release mode

cmake -DTARGET_GROUP=release .. 
make

# How to run the test 

1. build the software system in testing mode

cmake -DTARGET_GROUP=test .. 
make

2. run the test suite/sequence

ctest --verbose    


