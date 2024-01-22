-- The Glorious Glasgow Haskell Compilation System, version 9.4.7
import System.IO
import System.Exit
import System.Random
import System.IO.Unsafe
import qualified Crypto.Cipher.RC4 as RC4
import Data.ByteString (ByteString)
import qualified Data.ByteString as B
import qualified Data.ByteString.Char8 as C
import Data.Text.Encoding (encodeUtf8)
import Data.Char (ord)
import System.Process
import System.Environment
import Data.List.Split

packStr :: String -> B.ByteString
packStr = B.pack . map (fromIntegral . ord)

getVal :: Int -> Int -> String -> String
getVal m n str = take m (drop n str)

makeStr :: String -> [Char]
makeStr inp = Prelude.takeWhile (>= '\1') inp

len :: String -> Int
len inp = Prelude.length $ makeStr inp

strncmp :: String -> String -> Int -> Bool
strncmp a b c = (getVal c 0 a) == (getVal c 0 b)

toInt :: String -> Int
toInt inp = do
    let inp' = Prelude.filter (\x -> x > '/' && x < ':') inp
    Prelude.read inp' :: Int

encrypt :: B.ByteString -> B.ByteString -> (RC4.State, ByteString)
encrypt text key = RC4.combine (RC4.initialize key) text

genKey :: String
genKey = take 128 $ randomRs ('a','z') $ unsafePerformIO newStdGen

main = do
    
    hSetBuffering stdin NoBuffering
    hSetBuffering stdout NoBuffering
    hSetBuffering stderr NoBuffering
    
    let key = genKey
    let allowedCommand = ["echo", "uname", "id", "ls", "du", "ps", "cat", "base64", "cp", "touch", "echo"]
    let allowedArgument = ["'vaints'", "-n", "-u", "/var/backups", "-h", "-m", "/etc/passwd", "/etc/passwd", "--help", "/tmp/zzz", key]
    let packedKey = packStr key

    let loop = do
            putStrLn ""
            putStrLn "========= Command Executor [BETA] ========="
            putStrLn "1. View Allowed Command and Argument"
            putStrLn "2. Execute Command"
            putStrLn "3. Debug Signature Verification (development environment)"
            putStrLn "4. Exit"
            putStr "Input Choice : "
           
            choice <- getLine         
            case choice of
                "1" -> do
                    putStr "Enter Command ID : "
                    input <- getLine
                    let indexVal = toInt input
                    if (len input == 1) then do
                        putStrLn "=======[ALLOWED COMMAND]======="
                    else do
                        putStrLn "[RESPONSE] : Invalid index!"
                        loop
                    putStrLn $ "[RESPONSE] : " ++ ((allowedCommand !! indexVal) ++ " " ++ (allowedArgument !! indexVal))
                    loop
                "2" -> do
                    putStr "Enter Command ID : "
                    input <- getLine
                    putStr "Enter Command Argument : "
                    inputArgument <- getLine
                    if (len input == 1) then do
                        let indexVal = toInt input
                        if (indexVal >= 0 && indexVal < 10) then do
                            
                            let tmpInput = (allowedCommand !! indexVal) ++ inputArgument
                            let packedInput = packStr tmpInput

                            let tmpAllowed = (allowedCommand !! indexVal) ++ (allowedArgument !! indexVal)
                            let packedAllowed = packStr tmpAllowed
                            
                            let (newState1, encrypted1) = encrypt packedInput packedKey
                            let strEncrypted1 = C.unpack encrypted1
                            let (newState2, encrypted2) = encrypt packedAllowed packedKey
                            let strEncrypted2 = C.unpack encrypted2
                            
                            if (strncmp strEncrypted1 strEncrypted2 (len strEncrypted1)) then do
                                if len tmpAllowed /= len tmpInput then do
                                    putStrLn "[RESPONSE] Ups, something weird"
                                    putStr "Input Key to Validate : "
                                    inputKey <- getLine
                                    if inputKey == key then do
                                        let stdin' = ""
                                        (errCode, stdout', stderr') <- readProcessWithExitCode (allowedCommand !! indexVal) (splitOn " " inputArgument) stdin'
                                        putStrLn "[RESPONSE] : Signature verified!"
                                        putStrLn $ "[RESPONSE] : " ++ stdout'
                                    else do
                                        putStrLn "[RESPONSE] : Ciphertext wrong!"
                                else do
                                    let stdin' = ""
                                    (errCode, stdout', stderr') <- readProcessWithExitCode (allowedCommand !! indexVal) (splitOn " " inputArgument) stdin'
                                    putStrLn "[RESPONSE] : Signature verified!"
                                    putStrLn $ "[RESPONSE] : " ++ stdout'
                            else do
                               putStrLn "[RESPONSE] : Signature wrong!"
                               loop
                        else do
                           putStrLn "[RESPONSE] : Invalid index!"
                           loop
                    else do
                        putStrLn "[RESPONSE] : Invalid index!"
                        loop
                    loop
                "3" -> do
                    debugging <- getEnv "DEBUGGING_HASKELL"
                    if debugging == "true" then do
                        putStrLn $ "[RESPONSE] : " ++ ((allowedCommand !! 10) ++ " " ++ (allowedArgument !! 10))
                    else do
                        putStrLn "[RESPONSE] : Not in debugging mode"
                        loop
                    loop
                "4" -> do
                    putStrLn "[RESPONSE] : See you!"
                    exitSuccess 
                _ -> do
                    putStrLn "[RESPONSE] : Invalid choice!"
                    loop
    loop