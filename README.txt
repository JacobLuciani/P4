NOTE: In Project 3, the some operator tokens were incorrectly not stored on the parse tree. This has been corrected.

Program tested with included files: mini.sp2020 and tests.sp2020.
Both contain a series of commented-out programs, each of which was uncommented and processed
All programs that caused issues were resolved, except for <M>-generated *<M> portions,
Ex: 2 - *3
which I was unsure how to process. Nevertheless code was generated succesfully for these situations as well

One issue encountered and resolved was that only the first of any chain of vars would have storage allocated. This was resolved by fixing an omission of a recursive call to recGen after storing the first variable's informaiton
I did have one unreproducible bug where a code file was generated despite a missing colon on an assignment statemnt. The assignment statement was ignored. I believe it was an issue with how I was modifying the file, but I can't be sure


To compile, run "make" command.
