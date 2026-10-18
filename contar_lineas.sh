#!/bin/bash
path="${1:-.}"
find "$path" -name "*.cpp" -type f -exec wc -l {} +;
