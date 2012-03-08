#!/bin/bash

remote_addr='git@github.com:Erfan/Qsniff.git'
remote_name='Erfan'
remote_branch='master'
push_com='push -u'
pull_com='pull'

# Default answer is "NO"
temp_ans='N'

init_script="@@@ GIT script for Qsniff sniffer by Qazal company @@@"
echo  $init_script

echo  "Are you shore to start new git operation? (Y/N)"
read -t 3 temp_ans
if [ $temp_ans != "Y" ]
then
	exit 1
fi
temp_ans='N'
git status -u
echo -n  "Are you shore to continue git operation? (Y/N)"
read -t 3 temp_ans
if [ $temp_ans != 'Y' ]
then
	exit 1
fi
# first fetch and merge new updates from remote repo
git $pull_comm $remote_name $remote_branch
echo -n  "Are you shore to continue git operation? (Y/N)"
read -t 3 temp_ans
if [ $temp_ans != 'Y' ]
then
	exit 1
fi
# then push and upload new changes from local to remote
git $push_comm $remote_name $remote_branch


