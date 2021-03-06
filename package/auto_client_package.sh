#!/bin/bash
echo " "
echo "Start pack..."

# file pat
total_dir=./vc_linux_client

inside_tar="$total_dir""/vc_linux_client.tar.gz"

tmp_path="./bin ./etc ./lib ./log ./script"

if [ ! -d "$total_dir" ] 
then
    echo "error: $total_dir does not exist"
exit 1
fi

if [ ! -d "./bin" ] 
then
    mkdir $tmp_path
fi

cp ../build/main_daemon ./bin/vc_client_daemon

cp -rf ../build/virus_lib ./bin > /dev/null 2>&1

# Easy to modify with Python
cp -f ../etc/linux_client.xml $total_dir 

cp -f ../etc/vc_daemon_config.xml ./etc > /dev/null 2>&1

cp ../lib/*.so* ./lib > /dev/null 2>&1

cp -rf ../lib/engine_lib ./lib > /dev/null 2>&1

cp $total_dir/*client.sh $total_dir/uninstall.sh ./script

sed -i "31c \  sleep 5  #for reboot" ./script/run_vc_client.sh

echo "Wait a minute..."

tar zcf $inside_tar $tmp_path 

rm -rf  $tmp_path

tar zcf $total_dir"64bit"$(date +_%m%d)".tar.gz" $total_dir

echo "Finish"
echo " "
