#!/bin/bash


# Default answer is "NO"
temp_ans='N'

add_command='add -A'
commit_command='commit -m'

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

# add changes to index list of git repository
git $add_command

echo  "Please write your comment about your change in our repository:"
read comment_str

git $commit_command "$comment_str"
