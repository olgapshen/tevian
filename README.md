# tevian

## Test task for tevian

Compile project and build debian package with: `./make_deb.sh` from root repository directory.  

Install docker image:  

```
sudo docker build -t tevian .
```

Run container:

```
sudo docker run -it --entrypoint /bin/bash tevian
```

Run the command:

```
tevian_cli -h
```
