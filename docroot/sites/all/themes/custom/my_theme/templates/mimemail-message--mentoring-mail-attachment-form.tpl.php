<?php

/**
 * @file
 * Default theme implementation to format an HTML mail.
 *
 * Copy this file in your default theme folder to create a custom themed mail.
 * Rename it to mimemail-message--[module]--[key].tpl.php to override it for a
 * specific mail.
 *
 * Available variables:
 * - $recipient: The recipient of the message
 * - $subject: The message subject
 * - $body: The message body
 * - $css: Internal style sheets
 * - $module: The sending module
 * - $key: The message identifier
 *
 * @see template_preprocess_mimemail_message()
 */
?>
<html>
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <?php if ($css): ?>
    <style type="text/css"><?php print $css ?></style>
    <?php endif; ?>
  </head>
  <body>
  <table style="max-width: 600px" width="100%">
    <?php if (isset($message['logo_uri'])): ?>
    <tr>
      <td width="30%">
       <?php print theme('image', ['path' => $message['logo_uri']]); ?>
      </td>
    </tr>
    <?php endif;?>
    <tr>
      <td width="100%"><h1><?php print $message['subject']; ?></h1></td>
    </tr>
    <tr>
      <td width="100%"><?php print $body; ?></td>
    </tr>
  </table>
  </body>
</html>
