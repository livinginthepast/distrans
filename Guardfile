#!/usr/bin/env ruby
#^syntax detection

# Add files and commands to this file, like the example:
#   watch(%r{file/path}) { `command(s)` }
#
guard 'shell' do
  watch(/(.*).c/) {|m| `make` }
  watch(/Makefile/) {|m| `make` }
end
