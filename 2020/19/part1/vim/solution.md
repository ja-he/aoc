Split rules and messages manually.

Clean up rules manually:
- Remove space after colon and parenthesize:
  `:%s/^\(\d\+\): \(.*\)$/\1:(\2)/`
- Remove double quotes:
  `:%s/"//g`

Search pattern for lines with all resolved indices:
`^\d\+:\D*$`

What do we want do with a rule that is "fully resolved"?
Grab index and content, delete line, replace index everywhere with content:
`0"iyiwf("cya)dd:%s/\<i\>/c/g`

Do the above (let's say in register `r`) for every line that is "fully
resolved":
`:%g/^\d\+:\D*$/norm! @r`
(do this until only one rule remains)

Once down to a single rule, remove the whitespace:
`:%s/\s*//g`

Now convert it to a valid regexp, by prepending a backslash to `(`, `)` and `|`:
`:%s/\((\|)\||\)/\\\1/g`

Since Vim only allows you 9 numbered regexp groups (identified by `\(` and `\)`)
we need to "escape" the opening paren with a `%`:
`:%s/(/%(/g`

Now we just add `^` to the beginning, `$` to the end of the line and we have our
full search pattern.
We can also write the rest of our command in our buffer, because the command
line gets pretty messy with a command this long.
The command states to delete every line that doesn't match the pattern:
`%g!/PATTERN/d`

Now every line that remains matches the pattern and we have our answer in the
number of lines remaining.
