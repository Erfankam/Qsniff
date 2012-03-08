#!/usr/bin/ruby1.9.1

require 'mavenlink'

allowed_per_page_value = 3

client = Mavenlink::Client.new(\
                         91197,\
                         '87f26234-15f4-d561-c941-d0e7a82b3c36')


while true:
    workspace_list = client.workspaces(:perpage => allowed_per_page_value)
    workspace_list.respond_to?("each")
        workspace_list.each do |workspace|
            event = workspace.events(per_page => 1)
            if event




