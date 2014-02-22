
desc "Compile"
file 'psh' => ['src/psh.o'] do |t|
  sh "cc -o #{t.name} #{t.prerequisites.join ' '}"
end

rule '.o' => ['.c'] do |t|
  sh "cc #{t.source} -c -o #{t.name}"
end

desc "Run"
task 'run' do |t|
  sh "./psh"
end

task 'default' => ['psh', 'run']
