# tevian

## Test task for tevian

Compile project and build debian package with: `./make_deb.sh` from root repository directory.  

GitHub [link](https://github.com/olgapshen/tevian)  

Install docker image:  

```
sudo docker build -t tevian .
```

Run container:

```
sudo docker run -it --entrypoint /bin/bash tevian
```

Go back to host OS in other terminal window.  
Find ID of the docker process:  


```
sudo docker ps
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