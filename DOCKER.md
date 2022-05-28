# Using Docker for ADSL-CPP

## Obtaining and Running the Docker Image
If you'd prefer to skip all the manual configuration for ADSL and just want to use it,
then a Docker image is probably what you're looking for.

Docker is an application that allows you to run prebuilt "containers" or "images."
I have created a custom Docker image based on Ubuntu 20.04 hosted on Docker Hub. Note: I have to manually update the images, so they may be behind the latest commits in the GitHub repository.

To install the Docker tool, see https://docs.docker.com/engine/install/ubuntu/

To copy the (~3GB) custom image, just do `docker pull ifriedri/adsl`. 

To run the container, just use `docker run -it ifriedri/adsl`.

## Saving Your Work via Copying

Because Docker is a self-contained system, it doesn't mount any parts of your actual file system by default.

But, you'd probably like to write your own code that uses ADSL, and also save the results of your work with ADSL.

Doing this is as simple as mounting a shared folder when you execute `docker run` and copying files back and forth
using the command line from the Docker container.

To mount a directory on your computer located at ~/Documents/MyStuff/MyCode/ to a directory on the container called ~/MyCode, just execute:

`docker run -v ~/Documents/MyStuff/MyCode:/home/noroot/MyCode -it ifriedri/adsl`

From within the container, you can then copy your code files back and forth into the adsl-cpp directory:

`cp -r ~/MyCode ~/adsl-cpp` to copy all the files in MyCode from your computer to the container.

Or, `mkdir ~/MyCode/Results; cp -r ~/adsl-cpp ~/MyCode/Results` to copy the entire adsl-cpp container folder to a new folder on your computer.

(Don't forget to change `compile.sh` to reflect the name of the source code you want to compile and what you want the name of the executable to be).

## Saving Your Work in a New Conainer

If you don't feel like copying files back and forth and have adequate disk space, just save a copy of the modified container!

To do this, after you're done with a working session, do `exit` from within the Docker container.

Then, run `sudo docker ps -a` to list all the containers and their ids.

Noting the id of the most recent image (at the top, corresponding to ifriedri/adsl-docker or the copy you're already working with), run the following command to make a local copy of all the changes you just made:

`sudo docker commit [CONTAINER_ID] [new_image_name]`

Now if you run `sudo docker images` the new image should show up!

Note that you're allowed to overwrite images - i.e. if you previously made a copy called `image-copy` then `[new_image_name]` in the `commit` command above can be `image-copy`, so you don't have to make endless working copies.

To resume your work, use the `docker run` command as stated above but instead of `ifriedri/adsl-docker`, use whatever you decided to name the container copy.
