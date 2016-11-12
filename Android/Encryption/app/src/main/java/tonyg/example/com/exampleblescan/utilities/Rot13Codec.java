package tonyg.example.com.exampleblescan.utilities;

/**
 * Encrypt and Decrypt plain text with AES
 * Shamelessly stolen from http://stackoverflow.com/a/17396786
 *
 * @author Tony Gaitatzis backupbrain@gmail.com
 * @date 2016-02-21
 */
public class Rot13Codec {

    public static String encrypt(String plainText) {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < plainText.length(); i++) {
            char c = plainText.charAt(i);
            if       (c >= 'a' && c <= 'm') c += 13;
            else if  (c >= 'A' && c <= 'M') c += 13;
            else if  (c >= 'n' && c <= 'z') c -= 13;
            else if  (c >= 'N' && c <= 'Z') c -= 13;
            sb.append(c);
        }
        return sb.toString();
    }

    public static String decrypt(String encryptedText) {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < encryptedText.length(); i++) {
            char c = encryptedText.charAt(i);
            if       (c >= 'a' && c <= 'm') c += 13;
            else if  (c >= 'A' && c <= 'M') c += 13;
            else if  (c >= 'n' && c <= 'z') c -= 13;
            else if  (c >= 'N' && c <= 'Z') c -= 13;
            sb.append(c);
        }
        return sb.toString();

    }
}
