#!/usr/bin/env ruby
#^syntax detection

# Add files and commands to this file, like the example:
#   watch(%r{file/path}) { `command(s)` }
#
guard 'shell', all_on_start: true do
  watch(/(.*).c$/) do |m|
    if system("make")
      n "#{m[0]} valid", "Make succeeded", :success
    else
      n "Error in #{m[0]}", "Make failed", :failed
    end
  end
  watch(/Makefile/) {|m| `make` }
end
