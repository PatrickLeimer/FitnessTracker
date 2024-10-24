# Use an official GCC image from Docker Hub
FROM gcc:latest

# Set the working directory inside the container
WORKDIR /usr/src/app

COPY . .

RUN gcc -o main main.c

CMD ["./main"]

# commands: docker build -t fitness-tracker-app .
# docker run -it fitness-tracker-app
# Run but it saves the information in binary files: docker run -it -v fitness-data:/usr/src/app/data fitness-tracker-app
# Do you wanna store information in an image, then use docker volumes ----------
# check docker volumes: docker volume ls
# inspect data in volume: docker volume inspect fitness-data


