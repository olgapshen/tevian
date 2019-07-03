# Tevian

> Powered by Koshka!!!

## Test task for tevian

GitHub [link](https://github.com/olgapshen/tevian)  
DockerHub [link](https://cloud.docker.com/repository/docker/theservercat/tevian) 

### Install docker image

From root repository directory:  

```
docker build -t theservercat/tevian .
```

### Run container

```
docker run -it theservercat/tevian /bin/bash
```

Go back to host OS in other terminal window.  
Find ID of the docker process:  

```
docker ps
```

Using the ID, copy some images while you attached to console:  

```
docker cp images/cats2.jpg <ID>:/root/tevian/images
```

### Run CLI

Go back to docker terminal.  
Run the command:  

```
tevian_cli -t <token> -s tevian/images
```