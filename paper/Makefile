#
# Latex-aware Makefile example
#
# This assumes your latex source is in paper.tex, and that your
# citations are in references.bib
#
# Jim Teresco
#
# Wed Nov 20 14:30:15 EST 2002
#
LATEXSOURCE=paper
REFERENCES=references.bib
PDFLATEX=pdflatex
LATEX=latex
BIBTEX=bibtex

.SUFFIXES: .tex .dvi .bbl .aux .pdf

pdffile::	$(LATEXSOURCE).bbl $(LATEXSOURCE).pdf

.tex.pdf:
	$(PDFLATEX) $<

$(LATEXSOURCE).dvi:	$(LATEXSOURCE).tex $(LATEXSOURCE).bbl
	$(LATEX) $(LATEXSOURCE)

$(LATEXSOURCE).bbl:	$(LATEXSOURCE).aux $(REFERENCES)
	$(LATEX) $(LATEXSOURCE)
	$(LATEX) $(LATEXSOURCE)
	$(BIBTEX) $(LATEXSOURCE)
	$(LATEX) $(LATEXSOURCE)
	$(LATEX) $(LATEXSOURCE)

$(LATEXSOURCE).aux:	$(LATEXSOURCE).tex
	$(LATEX) $(LATEXSOURCE)

.tex.dvi:
	$(LATEX) $<

clean::
	/bin/rm -f $(LATEXSOURCE).ps $(LATEXSOURCE).dvi \
	$(LATEXSOURCE).aux $(LATEXSOURCE).blg $(LATEXSOURCE).log \
	$(LATEXSOURCE).bbl $(LATEXSOURCE).pdf

display::
	xdg-open $(LATEXSOURCE).pdf
