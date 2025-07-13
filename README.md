# gp-airline-demo
This create a data set that can be used for queries modelling an airline business

# pre-req
sudo dnf install postgresql-devel 
sudo dnf install gcc

# compilation
gcc -o create_airline_demo generate_demo.c -lpq

# testing with some queries
psql -f test.sql airline-demo
