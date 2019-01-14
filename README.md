# homorphic_vanity
Vanity address and homomorphic encryption for a Coinhouse project 


## Installation guide

1- Clone the repository


2- Install the gmp library for bignumber operation (if you do not already have it).
``sudo apt install libgmp3-dev ``

3-Build the project:
``make``

4-Generate your public/private keys
``./generate_key``

5- Generate your vanity address 
``./generate_vanity ``
(will use the default pattern "po") or
``./generate_vanity your_pattern``

You can also test homomorphic properties with
``./generate_vanity your_pattern`` (should display ``1``)
