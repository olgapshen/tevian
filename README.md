# tevian

## Test task for tevian

Compile project and build debian package with: `./make_deb.sh` from root repository directory.  

GitHub [link](https://github.com/olgapshen/tevian)  
DockerHub [link](https://cloud.docker.com/repository/docker/theservercat/tevian) 

Install docker image:  

```
docker pull theservercat/tevian
```

Run container:

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
docker cp images/cats1.jpg <ID>:/var/package/
```

Go back to docker terminal.  
Run the command:

```
tevian_cli -t <token> -f /var/package/cats1.jpg
```