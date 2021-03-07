FROM gitpod/workspace-full
RUN sudo apt-get update && sudo apt-get install -y cmake valgrind gengetopt libgtest-dev