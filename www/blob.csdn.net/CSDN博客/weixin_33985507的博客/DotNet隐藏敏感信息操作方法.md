# DotNet隐藏敏感信息操作方法 - weixin_33985507的博客 - CSDN博客
2016年10月21日 17:10:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
    在项目中，有时候一些信息不便完全显示，只需要显示部分信息。现在提供一些隐藏敏感信息的操作方法，主要为对信息的截取操作：
     1.指定左右字符数量,中间的*的个数和实际长度有关：
```
/// <summary>
        /// 隐藏敏感信息
        /// </summary>
        /// <param name="info">信息实体</param>
        /// <param name="left">左边保留的字符数</param>
        /// <param name="right">右边保留的字符数</param>
        /// <param name="basedOnLeft">当长度异常时，是否显示左边 
        /// <code>true</code>显示左边，<code>false</code>显示右边
        /// </param>
        /// <returns></returns>
        public static string HideSensitiveInfo(string info, int left, int right, bool basedOnLeft = true)
        {
            if (string.IsNullOrEmpty(info))
            {
                throw new ArgumentNullException(info);
            }
            var sbText = new StringBuilder();
            var hiddenCharCount = info.Length - left - right;
            if (hiddenCharCount > 0)
            {
                string prefix = info.Substring(0, left), suffix = info.Substring(info.Length - right);
                sbText.Append(prefix);
                for (var i = 0; i < hiddenCharCount; i++)
                {
                    sbText.Append("*");
                }
                sbText.Append(suffix);
            }
            else
            {
                if (basedOnLeft)
                {
                    if (info.Length > left && left > 0)
                    {
                        sbText.Append(info.Substring(0, left) + "****");
                    }
                    else
                    {
                        sbText.Append(info.Substring(0, 1) + "****");
                    }
                }
                else
                {
                    if (info.Length > right && right > 0)
                    {
                        sbText.Append("****" + info.Substring(info.Length - right));
                    }
                    else
                    {
                        sbText.Append("****" + info.Substring(info.Length - 1));
                    }
                }
            }
            return sbText.ToString();
        }
```
    2.指定左右字符数量,中间的*的个数固定：
```
/// <summary>
        /// 隐藏敏感信息
        /// </summary>
        /// <param name="info">信息实体</param>
        /// <param name="left">左边保留的字符数</param>
        /// <param name="right">右边保留的字符数</param>
        /// <param name="basedOnLeft">当长度异常时，是否显示左边 
        /// <code>true</code>显示左边，<code>false</code>显示右边
        /// <returns></returns>
        public static string HideSensitiveInfo1(string info, int left, int right, bool basedOnLeft = true)
        {
            if (string.IsNullOrEmpty(info))
            {
                throw new ArgumentNullException(info);
            }
            var sbText = new StringBuilder();
            var hiddenCharCount = info.Length - left - right;
            if (hiddenCharCount > 0)
            {
                string prefix = info.Substring(0, left), suffix = info.Substring(info.Length - right);
                sbText.Append(prefix);
                sbText.Append("****");
                sbText.Append(suffix);
            }
            else
            {
                if (basedOnLeft)
                {
                    if (info.Length > left && left > 0)
                    {
                        sbText.Append(info.Substring(0, left) + "****");
                    }
                    else
                    {
                        sbText.Append(info.Substring(0, 1) + "****");
                    }
                }
                else
                {
                    if (info.Length > right && right > 0)
                    {
                        sbText.Append("****" + info.Substring(info.Length - right));
                    }
                    else
                    {
                        sbText.Append("****" + info.Substring(info.Length - 1));
                    }
                }
            }
            return sbText.ToString();
        }
```
    3.“*”数量一定，设置为4个，按信息总长度的比例来取，默认左右各取1/3：
```
/// <summary>
        /// 隐藏敏感信息
        /// </summary>
        /// <param name="info">信息</param>
        /// <param name="sublen">信息总长与左子串（或右子串）的比例</param>
        /// <param name="basedOnLeft"/>当长度异常时，是否显示左边，默认true，默认显示左边
        /// <code>true</code>显示左边，<code>false</code>显示右边
        /// <returns></returns>
        public static string HideSensitiveInfo(string info, int sublen = 3, bool basedOnLeft = true)
        {
            if (string.IsNullOrEmpty(info))
            {
                throw new ArgumentNullException(info);
            }
            if (sublen <= 1)
            {
                sublen = 3;
            }
            var subLength = info.Length / sublen;
            if (subLength > 0 && info.Length > (subLength * 2))
            {
                string prefix = info.Substring(0, subLength), suffix = info.Substring(info.Length - subLength);
                return prefix + "****" + suffix;
            }
            if (basedOnLeft)
            {
                var prefix = subLength > 0 ? info.Substring(0, subLength) : info.Substring(0, 1);
                return prefix + "****";
            }
            var suffixs = subLength > 0 ? info.Substring(info.Length - subLength) : info.Substring(info.Length - 1);
            return "****" + suffixs;
        }
```
   4.隐藏右键详情
```
/// <summary>
        /// 隐藏右键详情
        /// </summary>
        /// <param name="email">邮件地址</param>
        /// <param name="left">邮件头保留字符个数，默认值设置为3</param>
        /// <returns></returns>
        public static string HideEmailDetails(string email, int left = 3)
        {
            if (string.IsNullOrEmpty(email))
            {
                throw new ArgumentNullException(email);
            }
            if (!System.Text.RegularExpressions.Regex.IsMatch(email, @"\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*"))
                return HideSensitiveInfo(email);
            var suffixLen = email.Length - email.LastIndexOf('@');
            return HideSensitiveInfo(email, left, suffixLen, false);
        }
```
    在一些信息的隐藏操作，可以采用js实现，也可以采用jquery插件完成操作，但是在前台进行这样的操作，存在一些风险，在后台完成对字符的信息截取，可以很好的对信息进行保护。
