#bin\bash

#sudo route -A inet6 add aaaa::/64 gw bbbb::100

did=00124B0007C3B5BB

e=bbbb::108d:4e5d:58de:3c94

topico=temp

cd '/Dropbox/mqtt-sn-tools/'

echo registrando no topico de temperatura

./mqtt-sn-sub -h $e -d -v -t $did/$topico
