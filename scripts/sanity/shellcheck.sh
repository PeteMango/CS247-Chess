#!/bin/bash

cd ..

# run check on all sh files
# shellcheck disable=SC1073,SC1126
git ls-files '*.sh' | while IFS= read -r file; do
  shellcheck "$file"
done