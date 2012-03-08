#!/usr/bin/ruby

require 'rubygems'
require 'mavenlink'

client = Mavenlink::Client.new(91197, '87f26234-15f4-d561-c941-d0e7a82b3c36')

workspaces = client.workspaces(:per_page => 1)
puts "number of workspaces is #{workspaces.length}"
events = client.events(:per_page => 0)

if workspaces.respond_to?("each") 
	workspaces.each do |workspace|
		puts "workspace.json => #{workspace.json}\n\n"
		#puts "workspace.methods = #{workspace.methods}\n\n"
		#puts "workspace.stories = #{workspace.stories.length}\n\n"
	end
end

stories = workspaces[0].stories(:per_page => 0)
if stories.respond_to?("each") 
	stories.each do |story|
		#puts "story.json => #{story.json}\n\n"
		#puts "story.fields = #{story.methods}\n\n"
	end
end

if events.respond_to?("each")
	events.each do |event|
		#puts "event.method = #{event.methods}\n\n"
		#puts "event.json => #{event.json}\n\n"
	end
end

posts = workspaces[0].posts(:per_page => 5)
if posts.respond_to?("each")
	posts.each do |post|
		#puts "post = #{post}"
		#puts "post.method = #{post.methods}\n\n"
		puts "post.json => #{post.json}\n\n"
	end
end
