## Maven

```
    ~/.m2/settings.xml      This is a reference for the user-specific configuration for Maven.
    mvn install             download plugin maven need
    mvn package             compile
    ~/.m2/                  When maven build is executed, Maven automatically downloads all the dependency jars into the local repository.
    mvn package -Dmaven.test.skip=true      skip simple test
    mvn package -Dmaven.javadoc.skip=true   skip doc generator
```
