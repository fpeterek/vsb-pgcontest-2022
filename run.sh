#!/bin/bash

WORKLOAD=${1:-mini}

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

cd "$DIR"/workloads/"${WORKLOAD}" || exit 1
"${DIR}"/build/contest < input
