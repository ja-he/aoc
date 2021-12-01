import System.IO (isEOF)
import Data.List

main = myRead []

myRead l = do
  done <- isEOF
  let (a,b) = findPair l
  if done
    then
      print (a*b)
    else do
      i <- getLine
      myRead ((read i :: Integer):l)

findPair l =
  head $ filter (\(a,b) -> a+b == 2020) [(a,b) | a <- l, b <- l]
