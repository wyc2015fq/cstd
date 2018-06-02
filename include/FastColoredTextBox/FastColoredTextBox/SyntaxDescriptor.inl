using System.Collections.Generic;
using System.Text.RegularExpressions;
using System;


{
    struct SyntaxDescriptor: IDisposable
    {
        char leftBracket = '(';
        char rightBracket = ')';
        char leftBracket2 = '{';
        char rightBracket2 = '}';
        BracketsHighlightStrategy bracketsHighlightStrategy = BracketsHighlightStrategy.Strategy2;
        readonly List<Style> styles = new List<Style>();
        readonly List<RuleDesc> rules = new List<RuleDesc>();
        readonly List<FoldingDesc> foldings = new List<FoldingDesc>();

        void Dispose()
        {
            foreach (var style in styles)
                style.Dispose();
        }
    }

    struct RuleDesc
    {
        Regex regex;
        string pattern;
        RegexOptions options = RegexOptions.None;
        Style style;

        Regex Regex
        {
            get
            {
                if (regex == null)
                {
                    regex = new Regex(pattern, SyntaxHighlighter.RegexCompiledOption | options);
                }
                return regex;
            }
        }
    }

    struct FoldingDesc
    {
        string startMarkerRegex;
        string finishMarkerRegex;
        RegexOptions options = RegexOptions.None;
    }
}
