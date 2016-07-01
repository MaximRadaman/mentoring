/**
 * @file
 * Custom js functions
 */
(function ($) {
    /**
     * Ajax delivery command to reload page after delay.
     */
    Drupal.ajax.prototype.commands.mentaringReload = function (ajax, response, status) {
        if (response.mentoringOffset != 'underfined') {
            setTimeout(function() {
                location.reload();
            }, response.mentoringOffset);
        }
    };

})(jQuery);
