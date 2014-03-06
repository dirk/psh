
# LDFLAGS = '-lncurses'
LDFLAGS = '-lreadline'
CFLAGS = '-Wall -g'

desc "Compile dependencies"
task 'deps' => ['deps/linenoise/linenoise.o']


desc "Compile"
file 'psh' => [
  # Source
  'src/psh.o', 'src/reader.o', 'src/parser.o', 'src/exec.o', 'src/eval.o', 'src/debug.o'
  # Deps
  # 'deps/linenoise/linenoise.o'
] do |t|
  sh "cc -o #{t.name} #{t.prerequisites.join ' '} #{LDFLAGS} #{CFLAGS}"
end

rule '.o' => ['.c'] do |t|
  sh "cc #{t.source} -c #{CFLAGS} -o #{t.name}"
end

desc "Run"
task 'run' do |t|
  sh "./psh"
end

desc "Clean"
task 'clean' do |t|
  sh "rm -v **/*.o"
end

# desc "Compile tests"
file 'psh_test' => [
  # Sources
  'test/unit/test.o',
  # Main to ensure we have all of psh built
  'psh'
] do |t|
  test_without_psh = t.prerequisites - ['psh']
  psh_without_main = Rake::Task['psh'].prerequisites - ['src/psh.o']
  sh "cc -o #{t.name} #{test_without_psh.join ' '} #{psh_without_main.join ' '} #{LDFLAGS} #{CFLAGS + " -Wno-unused-variable"}"
end

namespace 'test' do
  desc "Run unit tests"
  task 'unit' => ['psh_test'] do
    sh "./psh_test"
  end
end

task 'default' => ['psh', 'run']
