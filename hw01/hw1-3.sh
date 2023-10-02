#!/bin/bash

# Read parent pid, child pid
while [ "$#" -gt 0 ]; do
  case "$1" in
    --parent)
      parent="$2"
      shift 2
      ;;
    --child)
      child="$2"
      shift 2
      ;;
    --path)
      path=1
      shift
      ;;
    *)
      echo "Unknown parameter passed: $1"
      exit 1
      ;;
  esac
done

# Check if parent or child is empty
if [ -z "$parent" ] || [ -z "$child" ]; then
  echo "Usage: $0 --parent PARENT_PID --child CHILD_PID [--path]"
  exit 1
fi

########
# TODO #
########

# Find the path of parent and child
path_arr=()
while [ "$child" -ne "1" ]; do # child pid is 1 when it is the init process
  path_arr+=("$child")
  child=$(ps -o ppid= -p "$child")
  if [ "$child" -eq "$parent" ]; then
    echo "Yes"
    if [ -n "$path" ]; then
      echo -n "$parent"
      for (( idx=${#path_arr[@]}-1 ; idx>=0 ; idx-- )); do
        echo -n " -> ${path_arr[idx]}"
      done
      echo ""
    fi
    exit 0
  fi
done

echo "No"
exit 0
