[![Build Status](https://travis-ci.com/datachi7d/powerfc.svg?branch=master)](https://travis-ci.com/datachi7d/powerfc)
[![Coverage Status](https://coveralls.io/repos/github/datachi7d/powerfc/badge.svg)](https://coveralls.io/github/datachi7d/powerfc)



 [Gitpod link](https://gitpod.io/#https://github.com/datachi7d/powerfc)



### Notes

 Start sim:

    ./utils/socat.sh
    ./build/src/tools/pfc_sim_dump  --serial /tmp/sim1 --data ./src/tests/test_memory_dump2.dat

 Read value:

    ./src/tools/pfc_read --serial /tmp/sim2 -c ../src/tests/test_memory_config.xml  -i 129
