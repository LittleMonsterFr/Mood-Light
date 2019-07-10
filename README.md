# Mood-Light

Personal project using the Matrix Creator board (https://www.matrix.one/products/creator) as a mood light controlled with Chirp's technology (https://chirp.io/).

**⚠️ The build process of the binaries which run on the Raspberry Pi is not supported on MacOS and hasn't been tested on Windows. Please use the pre-built files in `Mood-Light.zip` as the files to use on the RPI. On the other hand, the iOS app will require to own a MacOS based system with XCode to be able to build as the app is not on the App Store.⚠️**

## Pre requisites

**On the host (linux):**

- CMake 3.0.0 or higher.
- Git

Once you have cloned the repo you need to update the submodules. Go to the root of the project and run the following command :

```bash
git submodule update --init --recursive
```

The command will clone all the necessary submodules and files.

**On the Raspberry Pi :**

From the official documentation of Matrix.io (https://github.com/matrix-io/matrix-creator-hal) :

```bash
# Add repo and key
curl https://apt.matrix.one/doc/apt-key.gpg | sudo apt-key add -
echo "deb https://apt.matrix.one/raspbian $(lsb_release -sc) main" | sudo tee /etc/apt/sources.list.d/matrixlabs.list

# Update repo and packages
sudo apt-get update
sudo apt-get upgrade

# Install MATRIX HAL Packages
sudo apt-get install matrixio-creator-init libmatrixio-creator-hal libmatrixio-creator-hal-dev
```

## Build process

The build process is controlled by the bash script `run.bash`. This scripts takes care of building the files for the RPI and, if wanted, to copy them to the RPI. A very minimal documentation is accessible when calling :

```bash
./run.bash -h
```

The same documentation is displayed for any misuse of the script.

When copying the files to the RPI, you will need to create a folder called `mood-light` in the home folder of the RPI. If you want to use a different folder name, don't forget to update `DEST_FOLDER` in `run.bash`.

The RPI is found by its hostname which is assumed to be `raspberrypi.local`. If not, dont forget to change `RPI_HOSTNAME` to the wanted one in `run.bash`.

## Known Issues

- Trying to build in verbose mode in a Docker environment will fail because of the way the output is caught to then be displayed or just saved. This is not important and small modifications of `run.bash` can be made without breaking the build.

## Miscellaneous

Github : https://github.com/LittleMonsterFr

Original location of the repo : https://github.com/LittleMonsterFr/Mood-Light

I am open to any additions to add new cool patterns 😎 or to make the build process simpler or even to have an Android app ! Just create an issue and / or a pull request and I'll do my best to look at it quickly =).