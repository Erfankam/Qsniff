#!/bin/bash

remote_address='git@github.com:Erfan/Qsniff.git'
remote_name='Erfan'
remote_branch='master'
push_command='push -u'
pull_command='pull -u'

# Default answer is "NO"
temp_ans='N'

init_script="@@@ GIT script for Qsniff sniffer by Qazal company @@@"
echo  $init_script

echo  "Are you shore to start new git operation? (Y/N)"
read -t 5 temp_ans
if [ "$temp_ans" != 'Y' ]
then
	exit 1
fi
temp_ans='N'
git status -u
echo -n  "Are you shore to continue git operation? (Y/N)"
read -t 5 temp_ans
if [ "$temp_ans" != 'Y' ]
then
	exit 1
fi

# Set remote name to Our repository address
# and dont show the results
result=`git remote add $remote_name $remote_address`

# first fetch and merge new updates from remote repo
git $pull_command $remote_name $remote_branch
echo -n  "Are you shore to continue git operation? (Y/N)"
read -t 5 temp_ans
if [ "$temp_ans" != 'Y' ]
then
	exit 1
fi
# then push and upload new changes from local to remote
git $push_command $remote_name $remote_branch
