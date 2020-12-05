import System.IO (isEOF)
import Data.List

main = my_read []

my_read l = do
  done <- isEOF
  let (a,b) = find_pair l
  if done
    then
      putStrLn $ show (a*b)
    else do
      i <- getLine
      my_read ((read i :: Integer):l)

find_pair l =
  head $ filter (\(a,b) -> a+b == 2020) [(a,b) | a <- l, b <- l]
