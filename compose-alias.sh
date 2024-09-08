alias uros-lan='sudo docker run -it --net=host microros/micro-ros-agent:humble udp4 -p'
alias uros-serial='sudo docker run -it --rm -v /dev:/dev --privileged --net=host microros/micro-ros-agent:humble serial --dev /dev/ttyACM0 -b 115200'
