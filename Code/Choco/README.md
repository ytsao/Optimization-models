### Setup 

1. Go to choco solver repository (https://github.com/chocoteam/choco-solver/tree/master).
2. Choose the version of choco from Releases. 
3. Download jar file.
4. (Option) If you have to solve continuous satisfation problems, you have to install ibex. Go to ibex repository (https://github.com/ibex-team/ibex-lib).

```cmd 
git clone https://github.com/ibex-team/ibex-lib.git
cd ibex-lib 
mkdir build 
cd build 
cmake -DLP_LIB=soplex -DBUILD_JAVA_INTERFACE=ON -DBUILD_SHARED_LIBS=ON -DJAVA_PACAKGE=org.chocosolver.solver.constraints.real ..
make 
sudo make install 
```

After that, you can use `ibexsolve -v` to check if ibex is install correctly. 

5. (Option) If you don't have java environment before, you can use `JAVA_HOME=/usr/lib/jvm/java-21-openjdk-amd64` to set environment variable.
6. In Eclipse, after creating a project, you just need to add two jar files, one is from choco solver another one is for ibex, into Java Build Path - Libraries - Classpath. Then the environment for choco and ibex is built!
