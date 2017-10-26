We have agreed on the following development process.

  * Tasks to be done are taken note on the *Projects* tab on Github. Each of us has a board to which the tasks we're working on should be moved to, in that way we now what others are doing and can plan accordingly or avoid interfering. Tasks which need discussion, lengthy descriptions or that you want to address from commits should be turned into an issue.
  * Each time we begin working on a new task we create a new branch with `git checkout -b new_feature`. We work on that feature on that branch. When we're done we make sure everything compiles and everything is thoroughly tested. Now we're ready to merge the new code we implemented into the master branch, so that others can build on it. This should be done in class. For more details on how to do branching see the [Try git tutorial](https://try.github.io "Try git tutorial").
  * In any case, any code that you commit must compile and pass all tests, including the ones you should have written to test the new code you are pushing.

Note: working on a branch does not mean you can't push to Github, it just means you can't push to master, because that will affect all others who are working on the project. If you want to back up your changes you may push to your own branch by executing `git push origin your_branch_name`.
