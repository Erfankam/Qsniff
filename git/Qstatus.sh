#!/bin/bash

status_command="status -u"
# Default answer is "NO"
temp_ans='N'

# Echo states of whole repository recursivly.
#git status -u
git $status_command

