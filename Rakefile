
# LDFLAGS = '-lncurses'
LDFLAGS = '-lreadline'

desc "Compile dependencies"
task 'deps' => ['deps/linenoise/linenoise.o']


desc "Compile"
file 'psh' => [
  # Source
  'src/psh.o', 'src/reader.o',
  # Deps
  # 'deps/linenoise/linenoise.o'
] do |t|
  sh "cc -o #{t.name} #{t.prerequisites.join ' '} #{LDFLAGS}"
end

rule '.o' => ['.c'] do |t|
  sh "cc #{t.source} -c -Wall -o #{t.name}"
end

desc "Run"
task 'run' do |t|
  sh "./psh"
end

desc "Clean"
task 'clean' do |t|
  sh "rm -v **/*.o"
end

task 'default' => ['psh', 'run']